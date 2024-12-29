<script lang="ts" setup>
  import { ref, reactive, computed } from 'vue'
  import {Search, Refresh, Edit, Delete, Key, CircleCheck} from '@element-plus/icons-vue'
  import { User, UserQuery, getUserList } from '@/api/user'
  import { ElPagination } from 'element-plus'

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
    const res = await getUserList({
      ...queryParams,
      page: 1,
      pageSize: 10,
    })
    userList.value = res.data || []
    console.log(res)
  }

  const multiple = computed(() => {
    return userList.value.some(user => user.selected)
  })

  const handleStatusChange = (row: User) => {
    // TODO: update user status
  }

  const handleSelect = (selection: User[], row:User) => {
    row.selected = selection.includes(row)
  }

  const handleSelectAll = (selection: User[]) => {
    userList.forEach(user => {
      user.selected = selection.includes(user)
    })
  }

  const handleSelectionChange = (selection: User[]) => {
    userList.forEach(user => {
      user.selected = selection.includes(user)
    })
  }

  // 新增
  const handleAdd = () => {
    // TODO: add user
  }

  // 修改
  const handleUpdate = (row: User) => {
    // TODO: update user
  }

  // 删除
  const handleDelete = (row: User) => {
    // TODO: delete user
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

</script>

<template>
  <el-row :gutter="20">
    <el-col>
      <el-form :model="queryParams" ref="queryForm" size="default" :inline="true" label-width="68px">
        <el-form-item label="用户名称" prop="username">
          <el-input v-model="queryParams.username" placeholder="请输入用户名称" clearable style="width: 240px" @keyup.enter.native="handleQuery" />
        </el-form-item>
        <el-form-item label="手机号码" prop="phoneNumber">
          <el-input v-model="queryParams.phoneNumber" placeholder="请输入手机号码" clearable style="width: 240px" @keyup.enter.native="handleQuery" />
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-select v-model="queryParams.status" placeholder="用户状态" clearable style="width: 240px">
            <el-option label="正常" :value="0" />
            <el-option label="禁用" :value="1" />
          </el-select>
        </el-form-item>
        <el-form-item label="创建时间" prop="dateRange">
          <el-date-picker v-model="queryParams.dateRange" style="width: 240px" value-format="yyyy-MM-dd" type="daterange" range-separator="-" start-placeholder="开始日期" end-placeholder="结束日期"></el-date-picker>
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

      <el-table class="table" :data="userList" @select="handleSelect" @select-all="handleSelectAll" @selection-change="handleSelectionChange">
        <el-table-column type="selection" width="50" align="center" />
        <el-table-column label="id" align="center" key="userId" prop="userId" />
        <el-table-column label="用户名称" align="center" key="username" prop="username" :show-overflow-tooltip="true" />
        <el-table-column label="用户昵称" align="center" key="nickname" prop="nickname" :show-overflow-tooltip="true" />
        <el-table-column label="部门" align="center" key="deptName" prop="dept.deptName" :show-overflow-tooltip="true" />
        <el-table-column label="手机号码" align="center" key="phoneNumber" prop="phoneNumber" width="120" />
        <el-table-column label="状态" align="center" key="status">
          <template v-slot="{ row }">
            <el-switch v-model="row.status" active-value="0" inactive-value="1" @change="handleStatusChange(row)"></el-switch>
          </template>
        </el-table-column>
        <el-table-column label="创建时间" align="center" key="createTime" prop="createTime" width="160"/>
        <el-table-column label="操作" align="center" width="400" fixed="right">
          <template v-slot="{ row }">
            <el-button :icon="CircleCheck" type="primary" size="small" @click="handleAuthRole(row)">分配角色</el-button>
            <template v-if="row.userId !== 1">
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
