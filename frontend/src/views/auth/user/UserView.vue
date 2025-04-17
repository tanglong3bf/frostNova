<script lang="ts" setup>
import { ref, reactive, onMounted } from 'vue'
import { ElMessage } from 'element-plus'
import type { FormRules } from 'element-plus'
import '@/api/types/index.d.ts'
import { newUser, updateUser } from '@/api/user'
import UserQuery from './UserQuery.vue'
import UserTable from './UserTable.vue'
import UserDialog from './UserDialog.vue'

/// 基础数据 @{

/** 用户列表查询参数 */
const queryParams = reactive<UserQuery>({
  username: undefined,
  phoneNumber: undefined,
  status: undefined,
  dateRange: undefined,
})

/// @}

/**
 * 获取用户列表
 */
const userTable = ref<InstanceType<typeof UserTable> | null>(null)
const userDialog = ref<InstanceType<typeof UserDialog> | null>(null)
const getList = async () => {
  if (userTable.value) {
    await userTable.value.getList()
  }
}

const setUser = (user: User) => {
  if (userDialog.value) {
    userDialog.value.setUser(user)
  }
}

const setIsDialogVisible = (value: boolean) => {
  if (userDialog.value) {
    userDialog.value.setIsDialogVisible(value)
  }
}

onMounted(() => {
  getList()
})

/** 重置查询条件 */
const resetQuery = () => {
  queryParams.username = undefined
  queryParams.phoneNumber = undefined
  queryParams.status = undefined
  queryParams.dateRange = undefined
}

/** 新增/修改用户 */
const isAdd = ref(false)

</script>

<template>
  <el-row :gutter="20">
    <el-col>
      <UserQuery :queryParams="queryParams" @get-list="getList" @reset-query="resetQuery" />
      <UserTable ref="userTable" :queryParams="queryParams" @set-is-add="(value: boolean) => isAdd = value"
        @set-is-dialog-visible="setIsDialogVisible" @set-user="setUser" />
    </el-col>
  </el-row>
  <UserDialog ref="userDialog" :is-add="isAdd" @get-list="getList" />
</template>