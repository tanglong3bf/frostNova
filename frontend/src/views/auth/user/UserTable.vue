<script lang="ts" setup>
import { ref, computed } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Edit, Delete, Key } from '@element-plus/icons-vue'
import { getUserList, updateStatus, deleteUser, batchDeleteUser } from '@/api/user'
import '@/api/types/index.d.ts'

const props = defineProps({
  queryParams: {
    type: Object as () => UserQuery,
    required: true,
  }
})

const emits = defineEmits(['setIsAdd', 'setIsDialogVisible', 'setUser'])

const setIsAdd = (value: boolean) => {
  emits('setIsAdd', value)
}

const setIsDialogVisible = (value: boolean) => {
  emits('setIsDialogVisible', value)
}

const setUser = (user: User) => {
  emits('setUser', user)
}

/** 用户列表数据 */
const userList = ref<User[]>([])

/** 分页相关数据 */
const currentPage = ref(1)
const pageSize = ref(10)
const total = ref(0)

/**
 * 获取用户列表
 */
const getList = async () => {
  try {
    const filteredParams = Object.fromEntries(
      Object.entries(props.queryParams).filter(([key, value]) => value !== '' && value !== undefined && value !== null || Array.isArray(value) && value.length > 0)
    );
    const res: PaginateResponse<User> = await getUserList({
      ...filteredParams,
      page: currentPage.value,
      pageSize: pageSize.value,
    })
    userList.value = res.list || []
    total.value = res.total || 0
    currentPage.value = res.page || 1
    pageSize.value = res.pageSize || 10
  } catch (error) {
    ElMessage.error('获取用户列表失败')
  }
}

/** 计算属性：是否有选中项 */
const multiple = computed(() => {
  return userList.value.some(user => user.selected)
})

/** 切换单个用户启用状态 */
const handleStatusChange = async (row: User) => {
  try {
    await updateStatus(row.user_id, row.status)
  } catch (error) {
    ElMessage.error('状态修改失败')
    row.status = row.status === 0 ? 1 : 0
    return
  }
  ElMessage.success('状态修改成功')
}

/** 切换单个用户选中状态 */
const handleSelect = (selection: User[], row: User) => {
  row.selected = selection.includes(row)
}

/** 全选/取消全选 */
const handleSelectAll = (selection: User[]) => {
  userList.value.forEach(user => {
    user.selected = selection.includes(user)
  })
}

/** 任意一项或多项选中状态变化 */
const handleSelectionChange = (selection: User[]) => {
  userList.value.forEach(user => {
    user.selected = selection.includes(user)
  })
}

/** 更新用户按钮 */
const handleUpdate = (row: User) => {
  setUser(row)
  setIsAdd(false)
  setIsDialogVisible(true)
}

/** 删除用户按钮 */
const handleDelete = async (user_id: number) => {
  console.log(user_id)
  try {
    await ElMessageBox.confirm('确认删除该用户吗？', '提示', {
      confirmButtonText: '确定',
      cancelButtonText: '取消',
      type: 'warning',
    })

    const username = await ElMessageBox.prompt('请输入用户名以确认删除', '提示', {
      confirmButtonText: '确定',
      cancelButtonText: '取消',
      inputType: 'text',
      inputPlaceholder: '请输入用户名',
    })
    if (username.value !== userList.value.find(user => user.user_id === user_id)?.username) {
      ElMessage.error('用户名输入错误，删除失败')
      return
    }
  } catch (error) {
    console.error(error)
    if (error === 'cancel') {
      ElMessage.info('取消删除')
      return
    }
    ElMessage.error('删除失败')
    return
  }

  await deleteUser(user_id)

  ElMessage.success('删除成功')
  await getList()
}

/** 批量删除按钮 */
const handleBatchDelete = async () => {
  const ids = userList.value.filter(user =>
    user.selected).map(user => user.user_id)
  console.log(ids)
  let password: string
  try {
    await ElMessageBox.confirm('确认批量删除选中的用户吗？', '提示', {
      confirmButtonText: '确定',
      cancelButtonText: '取消',
      type: 'warning',
    })

    const passwd = await ElMessageBox.prompt('请输入密码以确认批量删除', '提示', {
      confirmButtonText: '确定',
      cancelButtonText: '取消',
      inputType: 'password',
      inputPlaceholder: '请输入密码',
    })
    password = passwd.value
  } catch (error) {
    console.error(error)
    if (error === 'cancel') {
      ElMessage.info('取消批量删除')
      return
    }
    ElMessage.error('批量删除失败')
    return
  }

  await batchDeleteUser(ids, password)

  ElMessage.success('批量删除成功')

  await getList()
}

/** 重置密码按钮 */
const handleResetPwd = (row: User) => {
  // TODO: reset user password
}

/** 修改每页数据量 */
const handleSizeChange = (size: number) => {
  pageSize.value = size
  getList()
}

/** 修改当前页 */
const handleCurrentChange = (page: number) => {
  currentPage.value = page
  getList()
}

/** 新增用户按钮 */
const handleAdd = () => {
  setIsDialogVisible(true)
  setIsAdd(true)
}

defineExpose({
  getList
})
</script>

<template>
  <el-row :gutter="10" class="mb8">
    <el-col :span="1.5">
      <el-button type="success" plain :icon="Edit" @click="handleAdd">新增</el-button>
    </el-col>
    <el-col :span="1.5">
      <el-button type="danger" plain :icon="Delete" :disabled="!multiple" @click="handleBatchDelete">批量删除</el-button>
    </el-col>
  </el-row>

  <el-table class="table" :data="userList" @select="handleSelect" @select-all="handleSelectAll"
    @selection-change="handleSelectionChange">
    <el-table-column type="selection" width="50" align="center" />
    <el-table-column label="id" align="center" key="user_id" prop="user_id" />
    <el-table-column label="用户名称" align="center" key="username" prop="username" :show-overflow-tooltip="true" />
    <el-table-column label="用户昵称" align="center" key="nickname" prop="nickname" :show-overflow-tooltip="true" />
    <el-table-column label="部门" align="center" key="dept.dept_name" prop="dept.deptName"
      :show-overflow-tooltip="true" />
    <el-table-column label="手机号码" align="center" key="phone" prop="phone" width="120" />
    <el-table-column label="状态" align="center" key="status">
      <template v-slot="{ row }">
        <!-- TODO: 通过角色判断是否禁用 -->
        <el-switch :disabled="row.user_id === 1" v-model="row.status" :active-value="0" :inactive-value="1"
          @change="handleStatusChange(row)"></el-switch>
      </template>
    </el-table-column>
    <el-table-column label="创建时间" align="center" key="create_time" prop="create_time" width="160" />
    <el-table-column label="操作" align="center" width="400" fixed="right">
      <template v-slot="{ row }">
        <template v-if="row.user_id !== 1">
          <el-button :icon="Key" type="warning" size="small" @click="handleResetPwd(row)">重置密码</el-button>
          <el-button :icon="Edit" type="primary" size="small" @click="handleUpdate(row)" />
          <el-button :icon="Delete" type="danger" size="small" @click="handleDelete(row.user_id)" />
        </template>
      </template>
    </el-table-column>
  </el-table>
  <div class="pagination-container">
    <el-pagination v-model:current-page="currentPage" v-model:page-size="pageSize" :page-sizes="[10, 20, 30, 50]"
      :total="total" :pager-count="7" layout="total, sizes, prev, pager, next, jumper" @size-change="handleSizeChange"
      @current-change="handleCurrentChange" />
  </div>
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