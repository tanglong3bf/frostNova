<script lang="ts" setup>
import {ref, reactive, computed, onMounted} from 'vue'
import {Search, Refresh, Edit, Delete, Key} from '@element-plus/icons-vue'
import {type User, type UserQuery, getUserList, type PaginateResponse, updateStatus } from '@/api/user'
import {ElPagination, ElMessage} from 'element-plus'

const queryParams = reactive<UserQuery>({
  username: undefined,
  phoneNumber: undefined,
  status: undefined,
  dateRange: [],
})

const handleQuery = () => {
  getList()
}

const resetQuery = () => {
  queryParams.username = undefined
  queryParams.phoneNumber = undefined
  queryParams.status = undefined
  queryParams.dateRange = []
}

const userList = ref<User[]>([])

const getList = async () => {
  const res: PaginateResponse<User> = await getUserList({
    ...queryParams,
    page: 1,
    pageSize: 10,
  })
  console.log(res)
  userList.value = res.list || []
}

onMounted(() => {
  getList()
})

const multiple = computed(() => {
  return userList.value.some(user => user.selected)
})

// const isInitialLoad = ref(true)
const handleStatusChange = async (row: User) => {
  // if (isInitialLoad.value) {
  //   isInitialLoad.value = false
  //   return
  // }

  try {
    await updateStatus(row.user_id, row.status)
  } catch (error) {
    ElMessage.error('状态修改失败')
    row.status = row.status === 0 ? 1 : 0
    return
  }
  ElMessage.success('状态修改成功')
}

const handleSelect = (selection: User[], row: User) => {
  row.selected = selection.includes(row)
}

const handleSelectAll = (selection: User[]) => {
  userList.value.forEach(user => {
    user.selected = selection.includes(user)
  })
}

const handleSelectionChange = (selection: User[]) => {
  userList.value.forEach(user => {
    user.selected = selection.includes(user)
  })
}

// 修改
const handleUpdate = (row: User) => {
  // TODO: update user
}

// 删除
const handleDelete = (row: User) => {
  // TODO: delete user
}

const handleResetPwd = (row: User) => {
  // TODO: reset user password
}

const currentPage = ref(1)
const pageSize = ref(10)
const total = ref(0)

const handleSizeChange = (size: number) => {
  pageSize.value = size
  getList()
}

const handleCurrentChange = (page: number) => {
  currentPage.value = page
  getList()
}

/// dialog
const isDialogVisible = ref(false)
const isAdd = ref(false)

const user = reactive<User>({
  user_id: 0,
  username: '',
  nickname: '',
  phone: '',
  status: 0,
  create_time: '',
  dept: {
    dept_name: '',
  },
  selected: false,
})

const rules = reactive<FormRules<User>>({
  username: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    { min: 8, max: 32, message: '长度在 8 到 32 个字符', trigger: 'blur' },
    { pattern: /^[a-zA-Z0-9]+$/, message: '用户名只能包含字母和数字', trigger: 'blur' }
  ]
});

// 新增
const handleAdd = () => {
  isDialogVisible.value = true
  isAdd.value = true
}

// 关闭弹窗
const handleClose = () => {
  isDialogVisible.value = false
}

// 保存
const handleSave = async () => {
  // TODO: save user
  isDialogVisible.value = false
}

</script>

<template>
  <el-row :gutter="20">
    <el-col>
      <el-form :model="queryParams" ref="queryForm" size="default" :inline="true" label-width="68px">
        <el-form-item label="用户名称" prop="username">
          <el-input v-model="queryParams.username" placeholder="请输入用户名称" clearable style="width: 240px"
                    @keyup.enter.native="handleQuery"/>
        </el-form-item>
        <el-form-item label="手机号码" prop="phoneNumber">
          <el-input v-model="queryParams.phoneNumber" placeholder="请输入手机号码" clearable style="width: 240px"
                    @keyup.enter.native="handleQuery"/>
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-select v-model="queryParams.status" placeholder="用户状态" clearable style="width: 240px">
            <el-option label="正常" :value="0"/>
            <el-option label="禁用" :value="1"/>
          </el-select>
        </el-form-item>
        <el-form-item label="创建时间" prop="dateRange">
          <el-date-picker v-model="queryParams.dateRange" style="width: 240px" value-format="YYYY-MM-DD"
                          type="daterange" range-separator="-" start-placeholder="开始日期"
                          end-placeholder="结束日期"></el-date-picker>
        </el-form-item>
        <el-form-item>
          <el-button type="primary" :icon="Search" size="small" @click="handleQuery">搜索</el-button>
          <el-button :icon="Refresh" size="small" @click="resetQuery">重置</el-button>
        </el-form-item>
      </el-form>

      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button type="success" plain :icon="Edit" @click="handleAdd">新增</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button type="danger" plain :icon="Delete" :disabled="!multiple" @click="handleDelete">批量删除</el-button>
        </el-col>
      </el-row>

      <el-table class="table" :data="userList" @select="handleSelect" @select-all="handleSelectAll"
                @selection-change="handleSelectionChange">
        <el-table-column type="selection" width="50" align="center"/>
        <el-table-column label="id" align="center" key="user_id" prop="user_id"/>
        <el-table-column label="用户名称" align="center" key="username" prop="username" :show-overflow-tooltip="true"/>
        <el-table-column label="用户昵称" align="center" key="nickname" prop="nickname" :show-overflow-tooltip="true"/>
        <el-table-column label="部门" align="center" key="dept.dept_name" prop="dept.deptName"
                         :show-overflow-tooltip="true"/>
        <el-table-column label="手机号码" align="center" key="phone" prop="phone" width="120"/>
        <el-table-column label="状态" align="center" key="status">
          <template v-slot="{ row }">
            <!-- TODO: 通过角色判断是否禁用 -->
            <el-switch :disabled="row.user_id === 1" v-model="row.status" :active-value="0" :inactive-value="1"
                       @change="handleStatusChange(row)"></el-switch>
          </template>
        </el-table-column>
        <el-table-column label="创建时间" align="center" key="create_time" prop="create_time" width="160"/>
        <el-table-column label="操作" align="center" width="400" fixed="right">
          <template v-slot="{ row }">
            <template v-if="row.user_id !== 1">
              <el-button :icon="Key" type="warning" size="small" @click="handleResetPwd(row)">重置密码</el-button>
              <el-button :icon="Edit" type="primary" size="small" @click="handleUpdate(row)"/>
              <el-button :icon="Delete" type="danger" size="small" @click="handleDelete(row)"/>
            </template>
          </template>
        </el-table-column>
      </el-table>
      <div class="pagination-container">
        <el-pagination
            v-model:current-page="currentPage"
            v-model:page-size="pageSize"
            :page-sizes="[10, 20, 30, 50]"
            :total="total"
            :pager-count="7"
            layout="total, sizes, prev, pager, next, jumper"
            @size-change="handleSizeChange"
            @current-change="handleCurrentChange"
        />
      </div>
    </el-col>
  </el-row>

  <el-dialog :title="isAdd ? '新增用户' : '修改用户'" v-model="isDialogVisible" width="400px" :before-close="handleClose">
    <el-form ref="userForm" :rules="rules" :model="user" label-width="80px">
      <el-form-item label="用户名" prop="username">
        <el-input v-model="user.username" placeholder="请输入用户名" clearable style="width: 300px"/>
        <el-tag type="warning">默认密码：123456</el-tag>
      </el-form-item>
      <el-form-item label="昵称" prop="nickname">
        <el-input v-model="user.nickname" placeholder="请输入昵称" clearable style="width: 300px"/>
      </el-form-item>
      <el-form-item label="手机号码" prop="phone">
        <el-input v-model="user.phone" placeholder="请输入手机号码" clearable style="width: 300px"/>
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button @click="handleClose">取 消</el-button>
      <el-button type="primary" @click="handleSave">保 存</el-button>
    </template>
  </el-dialog>
</template>

<style lang="scss" scoped>
.table {
  margin-top: 20px;
}

.pagination-container {
  margin-top: 20px;
  display: flex;
  justify-content: flex-end;
}
</style>
